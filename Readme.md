# AgileLB

AgileLB 是一个轻量、高性能且易于使用的负载均衡器，使用 Python 编写，旨在提供简单的配置和高效的请求分发功能。

## 特性

- **轻量**：AgileLB 设计简洁，使用少量依赖，可以在几乎任何 Python 环境中运行。
- **高性能**：优化的算法和精简的代码，能够以异步方式高效处理大量请求。
- **易用**：简洁的配置方式，支持多种常用负载均衡策略。

## 安装

要使用 AgileLB，你可以直接从源代码安装：

```bash
git clone https://github.com/moyanj/AgileLB.git
cd AgileLB
git checkout main
pip install -r requirements.txt
```

## 性能测试
见[performance.md](performance.md)

## 使用

### 基本配置

AgileLB 可以通过一个简单的 YAML 配置文件进行设置。以下是一个示例配置文件 `config.yaml`：

```yaml
host: "0.0.0.0"
port: 8000

servers:
  "127.0.0.1:8000":
    type:
    uris:
      - "http://127.0.0.1:5000"
      - "http://127.0.0.1:5001"
```

完整示例见[lb.yml](lb.yml)

### 启动服务

使用以下命令启动 AgileLB：

```bash
python main.py
```

AgileLB将会使用当前目录下的`lb.yml`配置文件启动服务 (可通过`AL_CONFIG_PATH`环境变量控制)。

### 负载均衡策略

AgileLB 支持以下负载均衡策略：

- `RoundScheduling`: 轮询法，将请求按顺序分配给服务器。
- `RandomScheduling`: 随机法，随机选择一台服务器处理请求。
- `LeastRequestScheduling`: 最少连接法，将请求分配给当前连接最少的服务器。
- `IPHashScheduling`: IP哈希法，依照IP的哈希值分配服务器


## 许可证

AgileLB 使用 LGPL2.1 许可证。详情请参阅 [LICENSE](LICENSE) 文件。

## 联系

如果你有任何问题或建议，可以通过 [your-email@example.com](mailto:your-email@example.com) 联系我们。