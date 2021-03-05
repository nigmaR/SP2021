#!bin/bash
echo "Compress or Extract Files"
echo "This program could compress or extract files by tar command"
echo "Made by ROman Nigmatullin 727-1"
check=0
exit="y"
main()
	{
	while true; do
		echo "Do you want to Compress(1) or Extract(2) Files?"
		read var
		if [ $var -eq 1 ]
		then
			echo "Write dir or files to compress"
			read dir
			echo "Write an archive name"
			read name
			$(tar -cf "$name".tar "$dir")
			break
		elif [ $var -eq 2 ]
		then
			echo "Write name of file.tar"
			read fil
			tar -xf $fil
			break
		else
			echo "Write 1 or 2"
		fi
	done
	}
while [ $exit = "y" ]
	do
		main
		echo "Do you want to use programm again?n/y"
		read exit
	done
