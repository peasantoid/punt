(use 'io' 'cmp' 'math' 'flow' 'str' 'os')

(fput (stdout) 'hey\n')
(sleep 3)
(system 'date')
(fput (stdout) (sfmt (access '/etc/X11/xorg.conf' 'rx') '\n'))
