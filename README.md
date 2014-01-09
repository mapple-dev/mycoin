# MyCoin


## What is MyCoin? - Such coin
Mycoin is like Bitcoin, but based on Litecoin, and also much more cooler.
It's build for Gaga fans from a Gaga fan


## License - Much license
MyCoin is released under the terms of the MIT license. See [COPYING](COPYING)
for more information or see http://opensource.org/licenses/MIT.

## Development and contributions - omg developers
Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

## Very Much Frequently Asked Questions

### How much MYC can exist?
Total of 86,000,000 much coins 

### How get MYC?
Scrypt Proof of Work

100 coins per Block





### Wow How to make it

	sudo apt-get install build-essential \
                         libssl-dev \
                         libdb5.1++-dev \
                         libboost-all-dev \
                         libqrencode-dev \
                         libminiupnpc-dev

	cd src/
	make -f makefile.unix USE_UPNP=-
	
Create the Config File:

	Go to .mycoin and add a new file called mycoin.conf open it and add this :
	
		rpcuser=coin
		rpcpassword=coins
		addnode=0.0.0.0:55883 ( ping mapple-mycoin.zapto.org and add the ip where 0.0.0.0 is dont add this help)
		server=1
		daemon=1
		
	Now you are ready to start:
		cd mycoin/src
		./mycoind 
		./mycoind setgenerate true 16 
		For help write in terminal ./mycoind help

### The Wallet-QT

	qmake
	make
	./mycoin-qt !!!!! Dont run mycoind and mycoin-qt at same time, it dont work !!!!!

