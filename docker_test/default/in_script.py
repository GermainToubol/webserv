import requests


server_list = {
    "http://webserv.42.fr": [200, "/root/index_all.html"],
    "http://192.168.13.1": [200, "/root/index_hidden.html"],
    "http://in.webserv.42.fr": [200, "/root/index_hidden.html"]
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
