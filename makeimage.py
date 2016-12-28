from PIL import Image
from PIL import ImageDraw

def load_stars( filename ) :
    stars = []
    text = ""
    with open( filename, 'r' ) as f :
        lines = f.readlines()
    for l in lines :
        if l[0] == '#' :
            text += l[2:]
            continue
        data = l.split(' ')
        n = int(data[0])
        y = float(data[1])
        x = -float(data[2])
        z = float(data[3])
        m = float(data[4])
        if z > 0 :
            stars.append( (n,x,-y,m) )
    return (text,stars)

def makeimage( text, stars, filename, screensize=700, bgcolor = (20,20,65) ) :
    half = screensize / 2.0;
    starrysky = Image.new( 'RGB', (screensize,screensize), bgcolor )
    draw = ImageDraw.Draw(starrysky)
    for (n,x,y,m) in stars :
        if m > 5 :
            continue
        ex = x * half;
        ey = y * half;
        em = (7 - m) ** 1.5 * 0.35
        draw.ellipse( ((half+ex-em, half+ey-em), (half+ex+em, half+ey+em)),
                    fill=(250,255,130), outline=bgcolor )

    draw.text( (0,0), text, (150,150,150) )
    starrysky.save( filename )

if __name__ == "__main__" :
    (text,stars) = load_stars( "stars.txt" )
    makeimage( text, stars, "result.png")
