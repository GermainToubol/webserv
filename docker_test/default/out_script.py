import requests


server_list = {
    "http://webserv.42.fr": [200, "/root/index_all.html"],
    "http://192.168.12.1": [200, "/root/index_all.html"],
    "http://out.webserv.42.fr": [200, "/root/index_coucou.html"],
    "http://webserv.42.fr/test/index.html": [200, "/root/index_test.html"],
    "http://webserv.42.fr/a/b/index.html": [200, "/root/index_test.html"],
    "http://webserv.42.fr/c/index.html": [403, ""]
    }


class Test:
    def __init__(self, url, status, filename):
        self.url = url
        self.status = status
        self.filename = filename

    def basic_get_test(self):
        req = requests.get(self.url)
        print(f"{self.url}: ", end="")
        print(f"expect {self.status} got {req.status_code}: content ", end="")
        if self.filename:
            with open(self.filename, 'r') as expected:
                if expected.read() == req.text:
                    print("ok", end="")
                else:
                    print("ko", end="")
        print("")


def server_selection(servers):
    for page in servers:
        t = Test(page, *servers[page])
        t.basic_get_test()


def main(servers):
    print("Start testing".center(30, "="))
    server_selection(servers)
    print("End testing".center(30, "="))


if __name__ == "__main__":
    main(server_list)
