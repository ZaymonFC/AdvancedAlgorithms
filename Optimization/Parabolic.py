import math

def func(x):
    return 2 * math.sin(x) - x**2 / 10

def Parabolic(xlef, xrig, func):
    x = 1
    x1 = 0
    xmid = (xlef + xrig) / 2
    fxlef = func(xlef)
    fxmid = func(xmid)
    fxrig = func(xrig)

    while (abs(x - x1) > 3e-8):
        x1 = x
        top = (xmid - xlef) ** 2 * (fxmid - fxrig) - (xmid - xrig) ** 2 * (fxmid - fxlef)
        bot = (xmid - xlef) * (fxmid - fxrig) - (xmid - xrig) * (fxmid - fxlef)
        x = xmid - 0.5 * (top / bot)

        if (fxlef > fxrig):
            xrig = xmid
            fxrig = fxmid

        else:
            xlef = xmid
            fxlef = fxmid
        xmid = x
        fxmid = func(x)
    return x

print(Parabolic(1, 3, func))