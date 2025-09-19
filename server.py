import asyncio
import websockets

count = 0
clients = set()

async def handler(websocket):
    global count, clients
    # 接続を追加
    clients.add(websocket)

    try:
        async for _ in websocket:
            count += 1
            print("count:", count)
            await broadcast_count()
            
    finally:
        clients.remove(websocket)

async def broadcast_count():
    global clients, count
    if clients:
        message = str(count)
        await asyncio.gather(*(ws.send(message) for ws in clients))

async def main():
    async with websockets.serve(handler, "0.0.0.0", 8000):
        print("Server running at ws://localhost:8000")
        await asyncio.Future()  # 無限待機

if __name__ == "__main__":
    asyncio.run(main())
