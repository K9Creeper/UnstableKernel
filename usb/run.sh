clear

echo "--------------------------"
echo "Creating Your Image"
echo "--------------------------"

grub-mkrescue -o os.iso $(dirname $0)

echo "--------------------------"
echo "Completed Your Image"
echo "--------------------------"