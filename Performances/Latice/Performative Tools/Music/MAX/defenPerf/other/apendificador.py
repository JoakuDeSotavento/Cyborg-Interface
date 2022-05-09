texto1 = 'append '
texto2 = 'method-'
texto3 = '.wav, '

with open('method.txt', 'w') as f:

    for x in range(13):
        if x<10:
            yes = texto1 + texto2 + str(0) + str(x) + texto3
            f.write(yes)
        else:
            yes = texto1 + texto2 + str(x) + texto3
            f.write(yes)

    f.close()
    
