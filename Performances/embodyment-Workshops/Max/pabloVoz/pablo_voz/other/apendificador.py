texto1 = 'append '
texto2 = 'thisIsNot'
texto3 = '.aif, '

with open('texto2.txt', 'w') as f:

    for x in range(89):
        if x<10:
            yes = texto1 + texto2 + str(0) + str(x) + texto3
            f.write(yes)
        else:
            yes = texto1 + texto2 + str(x) + texto3
            f.write(yes)

    f.close()
    
