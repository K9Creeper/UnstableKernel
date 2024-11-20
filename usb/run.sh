clear

echo "--------------------------"
echo "Creating Your Image"
echo "--------------------------"

grub-mkrescue -o os.iso $(pwd)

echo "--------------------------"
echo "Completed Your Image"
echo "--------------------------"