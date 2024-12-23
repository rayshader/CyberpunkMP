import RGL, {Layout, ReactGridLayoutProps, WidthProvider, WidthProviderProps} from "react-grid-layout";
import {Component, createElement, Fragment, ReactElement, Suspense, useEffect, useRef, useState} from "react";
import {Alert, Card, CircularProgress, GlobalStyles} from "@mui/material";
import Box from "@mui/material/Box";
import {ErrorBoundary} from "react-error-boundary";
import {PluginManifest, PluginModule, PluginWidget} from "../../Data/PluginData.ts";
import {useAppDispatch, useAppSelector} from "../../Stores/AppStore.ts";
import {saveLayouts} from "../../Stores/StorageStore.ts";
import {GridMode} from "./Dashboard.tsx";

const GridLayout = WidthProvider(RGL);
type GridLayoutComponent = Component<ReactGridLayoutProps & WidthProviderProps, never, never>;

interface WidgetGridProps {
  readonly plugins: PluginModule[];
  readonly gridMode: GridMode;
}

interface WidgetLayout {
  readonly key: string;
  readonly component: ReactElement;
  readonly layout: Layout;
}

export default function WidgetGrid({plugins, gridMode}: WidgetGridProps) {
  const MARGIN: number = 24;
  const CELL_SIZE: number = 64;

  const $root = useRef<GridLayoutComponent | null>(null);

  const {layouts} = useAppSelector(state => state.storage);
  const dispatch = useAppDispatch();

  const [columns, setColumns] = useState<number>(28);
  const [grid, setGrid] = useState<WidgetLayout[]>([]);

  useEffect(() => {
    const grid: WidgetLayout[] = plugins.map((plugin, i) => {
      const key: string = getLayoutKey(plugin.manifest);
      const layout: Layout | undefined = layouts.find(cache => cache.i === key);

      return createLayout(plugin, layout, i);
    });

    setGrid(grid);
  }, [plugins]);

  useEffect(() => {
    if (!$root.current) {
      return;
    }
    // @ts-expect-error should force a type
    const rect: DOMRect = $root.current.elementRef.current.getBoundingClientRect();
    const columns: number = Math.round(rect.width / CELL_SIZE);
    const width: number = rect.width - MARGIN * columns;

    setColumns(Math.ceil(width / CELL_SIZE));
  }, [$root]);

  const createLayout = (plugin: PluginModule, cache: Layout | undefined, i: number): WidgetLayout => {
    const widget: PluginWidget = plugin.widget!;
    const layout: Layout = {
      ...widget.layout,
      i: getLayoutKey(plugin.manifest),
      x: 0,
      y: i,
      w: widget.layout.minW ?? 4,
      h: widget.layout.minH ?? 4,
      ...cache
    };

    return {
      key: layout.i,
      component: createElement(widget.component),
      layout: layout
    } as WidgetLayout;
  }

  const getLayoutKey = (manifest: PluginManifest): string => `${manifest.author}/${manifest.name}`;

  const onLayoutChange = (layouts: Layout[]) => {
    dispatch(saveLayouts(layouts));
  };

  const layout: Layout[] = grid.map(item => item.layout);
  const canEditLayout: boolean = gridMode === 'edit';
  const pointerEvents: string = gridMode === 'edit' ? 'none' : 'auto';

  return (
    <Fragment>
      <GlobalStyles
        styles={{
          '.react-grid-item.react-grid-placeholder': {
            backgroundColor: 'var(--mui-palette-secondary-main)',
          },
          '.react-grid-item:not(.resizing):hover': {
            cursor: canEditLayout ? 'grab' : 'auto',
          },
          '.react-grid-item.react-draggable-dragging:not(.resizing)': {
            cursor: 'grabbing',
          },
          '.react-grid-item.resizing': {
            cursor: 'se-resize',
          },
          '.react-grid-item > .react-resizable-handle::after': {
            borderColor: 'var(--mui-palette-secondary-main)',
          }
        }}
      />
      <GridLayout
        ref={$root}
        className="layout"
        margin={[MARGIN, MARGIN]}
        containerPadding={[0, 0]}
        cols={columns}
        rowHeight={CELL_SIZE}
        resizeHandles={['se']}
        layout={layout}
        isDraggable={canEditLayout}
        isResizable={canEditLayout}
        onLayoutChange={onLayoutChange}
      >
        {grid.map(widget => (
          <Card key={widget.key}>
            <ErrorBoundary fallbackRender={
              ({error}) => (
                <Alert severity="error" sx={{width: '100%', height: '100%', m: 'auto'}}>
                  Error while rendering widget: {error.message}
                </Alert>
              )
            }>
              <Box sx={{width: '100%', height: '100%', pointerEvents: pointerEvents}}>
                <Suspense fallback={<CircularProgress/>}>
                  {widget.component}
                </Suspense>
              </Box>
            </ErrorBoundary>
          </Card>
        ))}
      </GridLayout>
    </Fragment>
  );
}
