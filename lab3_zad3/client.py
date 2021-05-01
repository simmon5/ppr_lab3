import xmlrpc.client

dane = input("Podaj dane \n")
s = xmlrpc.client.ServerProxy('http://localhost:12345')
while len(dane)>128:
    dane = input("Podaj dane (mniej niz 128)\n")
s.sample.add(dane)

# Print list of available methods
#print(s.system.listMethods())
