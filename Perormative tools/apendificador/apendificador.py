texto1 = 'append '
texto2 = 'thisIsNot'
texto3 = '.aif, '

with open('this.txt', 'w') as f:

    for x in range(84):
        if x<10:
            yes = texto1 + texto2 + str(0) + str(x) + texto3
            f.write(yes)
        else:
            yes = texto1 + texto2 + str(x) + texto3
            f.write(yes)

    f.close()
    
