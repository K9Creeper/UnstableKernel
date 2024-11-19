clear

echo "--------------------------"
echo "Creating Your Image"
echo "--------------------------"

grub-mkrescue -o boot/os.iso iso

mv os.iso ../os.iso

echo "--------------------------"
echo "Completed Your Image"
echo "--------------------------"