// This file is automatically generated

#pragma warning disable CS8618
#pragma warning disable CS0219

using CyberpunkSdk;
using CyberpunkSdk.Types;

using CyberpunkSdk.Systems;

using CyberpunkSdk.Internal;

namespace Cyberpunk.Rpc.Client.Plugins
{
public static class EmoteClient
{
	[ClientRpc(Class = 17492243237040389260, Function = 5701494315076382889)]
	public static void TriggerEmote(ulong playerId, ulong serverId, string name)
	{
		ushort length = 0;
		var data = IBuffer.Create();
		try{
			data.WriteUint64(serverId);
			data.WriteString(name);
			CyberpunkSdk.Server.RpcManager.Call(playerId, 17492243237040389260, 5701494315076382889, data);
		} finally {
			IBuffer.Release(data);
		}
	}

}
}