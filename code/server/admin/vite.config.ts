import {defineConfig} from 'vite';
import react from '@vitejs/plugin-react';

// https://vite.dev/config/
export default defineConfig({
  plugins: [
    react()
  ],
  build: {
    rollupOptions: {
      output: {
        manualChunks: {
          'vendor-0': ['systemjs'],
          'vendor-1': [
            'react', 'react-dom', 'react-error-boundary', 'react-router',
            'react-redux', 'react-grid-layout', 'react-resizable'
          ],
          'vendor-2': ['@emotion/react', '@emotion/styled'],
          'vendor-3-0': ['@mui/material'],
          'vendor-3-1': ['@mui/icons-material', '@mdi/react'],
        }
      }
    }
  },
  server: {
    proxy: {
      '/api': 'http://localhost:11778'
    },
  },
});
