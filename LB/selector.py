import random
import time
import threading
import urllib
import mmh3

class BaseSelector:
    def __init__(self, servers: list, config):
        self.servers = servers
        self.config = config
        self.is_live_set = set(servers)
        self.is_run = True
        self.check_thread = threading.Thread(target=self.check, daemon=True)
        self.lock = threading.Lock()
        
    async def get(self, requests):
        raise TypeError("Cannot use BaseSelector as selector")
    
    @property
    def is_live(self):
        return list(self.is_live_set)
        
    def check_server(self, server):
        """检查单个服务器的状态，并更新活跃服务器列表。"""
        try:
            res = urllib.request.urlopen(urllib.parse.urljoin(server, self.config.get('check_path', '/')), timeout=self.config.get('timeout', 5))
            if 100 <= res.status <= 499:
                if server not in self.is_live_set:
                    with self.lock:
                        self.is_live_set.add(server)
            else:
                with self.lock:
                    self.is_live_set.discard(server)  # 使用discard避免抛出异常
        except:
            with self.lock:
                self.is_live_set.discard(server)

    def check(self):
        while self.is_run:
            for server in self.servers:
                self.check_server(server)
            time.sleep(self.config.get("check_interval", 5))  # 休眠5秒


class RoundScheduling(BaseSelector):
    def __init__(self, servers, config):
        super().__init__(servers, config)
        self.n = 0

    async def get(self, request):
        if len(self.is_live) < 1:
            return None

        if self.n >= len(self.is_live):
            self.n = 0

        server = self.is_live[self.n]
        self.n += 1
        return server


class RandomScheduling(BaseSelector):
    async def get(self, request):
        if len(self.is_live) < 1:
            return None
        return random.choice(self.is_live)


class LeastRequestScheduling(BaseSelector):
    def __init__(self, servers, config):
        super().__init__(servers, config)
        self.request_count = {s: 0 for s in servers}
        self.least_request_server = None

    async def update_least_request(self):
        # 更新最少请求的服务器
        min_requests = min(self.request_count.values())
        self.least_request_server = [
            server
            for server, count in self.request_count.items()
            if count == min_requests
        ]

    async def get(self, request):
        if len(self.is_live) < 1:
            return None
        if not self.least_request_server or not any(
            server in self.is_live for server in self.least_request_server
        ):
            self.update_least_request()
        if not self.least_request_server:
            return None
        # 选择最少请求服务器中的一个
        server = random.choice(self.least_request_server)
        # 更新请求计数
        self.request_count[server] += 1
        return server
        
class IPHashScheduling(BaseSelector):
    def hash(self, data:str):
        data = data.encode()
        h = mmh3.hash(data, signed= False)
        return h % len(self.servers)
        
    async def get(self, request):
        n = self.hash(request.ip)
        return self.servers[n]
        
        