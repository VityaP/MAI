#!/usr/bin/python
# -*- coding: utf-8

import sys
import struct
from PIL import Image
import ctypes

pal = [
	(0, 0, 0),			(128, 128, 128),	(192, 192, 192),	(255, 255, 255),
	(255, 0, 255),		(128, 0, 128),		(255, 0, 0),		(128, 0, 0),
	(205, 92, 92),		(240, 128, 128),	(250, 128, 114),	(233, 150, 122),
	(205, 92, 92),		(240, 128, 128),	(250, 128, 114),	(233, 150, 122),
	(173, 255, 47),		(127, 255, 0),		(124, 252, 0),		(0, 255, 0),
	(50, 205, 50),		(152, 251, 152),	(144, 238, 144),	(0, 250, 154),
	(0, 255, 127),		(60, 179, 113),		(46, 139, 87),		(34, 139, 34),
	(0, 128, 0),		(0, 100, 0),		(154, 205, 50),		(107, 142, 35),
	(128, 128, 0),		(85, 107, 47),		(102, 205, 170),	(143, 188, 143),
	(32, 178, 170),		(0, 139, 139),		(0, 128, 128)
]

def to_img_alfa(src, dst = None):
	fin = open(src, 'rb')
	(w, h) = struct.unpack('hi', fin.read(8))
	buff = ctypes.create_string_buffer(4 * w * h)
	fin.readinto(buff)
	fin.close()
	img = Image.new('RGB', (w, h))
	pix = img.load()
	offset = 0
	sp = len(pal)
	for j in xrange(h):
		for i in xrange(w):
			(_, _, _, a) = struct.unpack_from('cccc', buff, offset)
			pix[i, j] = pal[ord(a) % sp]
			offset += 4
	if dst:
		img.save(dst)
	else:
		img.show()


def to_img(src, dst = None):
	fin = open(src, 'rb')
	(w, h) = struct.unpack('hi', fin.read(8))
	buff = ctypes.create_string_buffer(4 * w * h)
	fin.readinto(buff)
	fin.close()
	img = Image.new('RGB', (w, h))
	pix = img.load()
	offset = 0
	for j in xrange(h):
		for i in xrange(w):
			(r, g, b, _) = struct.unpack_from('cccc', buff, offset)
			pix[i, j] = (ord(r), ord(g), ord(b))
			offset += 4
	if dst:
		img.save(dst)
	else:
		img.show()


def from_img(src, dst):
	img = Image.open(src)
	img = img.convert('RGB')
	(w, h) = img.size[0:2]
	pix = img.load()
	buff = ctypes.create_string_buffer(4 * w * h)
	offset = 0
	for j in xrange(h):
		for i in xrange(w):
			r = chr(pix[i, j][0])
			g = chr(pix[i, j][1])
			b = chr(pix[i, j][2])
			struct.pack_into('cccc', buff, offset, r, g, b, '\0')
			offset += 4;
	out = open(dst, 'wb')
	out.write(struct.pack('ii', w, h))
	out.write(buff.raw)
	out.close()

def main():
	argv = []
	alfa = show = False
	for arg in sys.argv[1:]:
		if arg == '-a':
			alfa = True
		elif arg == '-s':
			show = True
		else:
			argv.append(arg)
	if show:
		if len(argv) == 1 and argv[0].endswith('.data'):
			if alfa:
				to_img_alfa(argv[0])
			else:
				to_img(argv[0])
		else:
			print("Error argv")
	else:
		if len(argv) != 2:
			print("Error count argv")
			sys.exit(1)
		f1 = argv[0].endswith('.data')
		f2 = argv[1].endswith('.data')
		if not (f1 or f2):
			print("Error type argv or count")
			sys.exit(1)
		if f1:
			if alfa:
				to_img_alfa(argv[0], argv[1])
			else:
				to_img(argv[0], argv[1])
		else:
			if not alfa:
				from_img(argv[0], argv[1])
			else:
				print("Error alfa")
	
if __name__ == '__main__':
	main()
