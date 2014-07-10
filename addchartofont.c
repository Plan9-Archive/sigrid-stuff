#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>

void
main(int argc, char **argv)
{
	int fd, i;
	uchar tmp[6];
	Memimage *im, *n;
	Subfont *s;
	Fontchar *c;
	Rectangle r;
	Point p;

	if(argc < 2)
		exits("usage");
	if((fd = open(argv[1], OREAD)) < 0)
		sysfatal("open");
	if((im = readmemimage(fd)) == nil)
		sysfatal("readimage");
	seek(fd, 0, 0);
	if((s = readsubfont(display, argv[1], fd, 0)) == nil)
		sysfatal("readsubfont");
	if(argc < 3){
		print("%d chars\n", s->n);
		exits(nil);
	}
	memimageinit();
	if(argc > 3){
		if(strcmp("print", argv[2]) == 0){
			i = atoi(argv[3]);
			if(i >= s->n){
				fprint(2, "char %d out of range\n");
				exits("out of range");
			}
			c = &s->info[i];
			print("0x%04x x=%d [left=%d top=%d bottom=%d width=%d]\n",
				i, c->x, c->left, c->top, c->bottom, c->width);
			exits(nil);
		}
		if(strcmp("add", argv[2]) == 0 && argc > 7){
			i = atoi(argv[3]);
			if(i > s->n){
				fprint(2, "char %d out of range\n");
				exits("out of range");
			}
			c = &s->info[i];
			if(c->width != 0){
				fprint(2, "char %d already exists\n", i);
				exits("exists");
			}
			c->left = atoi(argv[4]);
			c->top = atoi(argv[5]);
			c->bottom = atoi(argv[6]);
			c->width = atoi(argv[7]);
			if(c->width < 1 || c->width > 255){
				fprint(2, "bad width: %d\n", c->width);
				exits("width");
			}
			for(i++; i < s->n; i++)
				s->info[i].x += c->width;
			r = im->r;
			r.max.x += c->width;
			n = allocmemimage(r, im->chan);
			memfillcolor(n, 0x000000ff);
			r.max.x = c->x;
			memimagedraw(n, r, im, ZP, nil, ZP, S);
			r = im->r;
			r.min.x = c->x + c->width;
			r.max.x += c->width;
			p.x = c->x;
			p.y = r.min.y;
			memimagedraw(n, r, im, p, nil, p, S);
			writememimage(1, n);
			writesubfont(1, s);
			exits(nil);
		}
	}
}
