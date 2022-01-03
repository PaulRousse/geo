import geom_computation

def turn_center():
    geom_computation.turn_center(0,0,0,1000, 1)    

if __name__ == '__main__':
    import timeit
    print(timeit.timeit("turn_center()", setup="from __main__ import test", number=1000000)/1000000)
