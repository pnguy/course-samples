// Merge Sort example

class Project0{

	private int[] merge(int[] array1, int[] array2){
		int[] finalArray = new int[array1.length+array2.length];
		int i = 0, j = 0, k =0;
		//Add whichever number is smallest from array1 and array2 to finalArray.
		while ((i <= (array1.length-1)) && (j <= (array2.length-1))){
			if (array1[i] <= array2[j]){
				finalArray[k] = array1[i];
				i++;
				k++;
			}
			else{
				finalArray[k] = array2[j];
				j++;
				k++;
			}
		}
		//Adds the remainder from either array1 or array2.
		if (i == (array1.length))
			while(j <= (array2.length-1)){
				finalArray[k] = array2[j];
				j++;
				k++;
			}
		else
			while(i <=(array1.length-1)){
				finalArray[k] = array1[i];
				i++;
				k++;
			}
		return finalArray;
	}
	
	public int[] mergeSort(int[] toSort){
		int[] sortedArray = new int[toSort.length];
		int[] b = new int[toSort.length/2];
		int[] a = new int[toSort.length-b.length];
		
		//Adds the first half of toSort to a and second half to b.
		int i =0;
		while (i < a.length){
			a[i] = toSort[i];
			i++;
		}
		
		int j = 0;
		while (i < toSort.length){
			b[j] = toSort[i];
			i++;
			j++;
		}
		//Recursive call for mergeSort, base case is when length is 1.
		if (a.length > 1){
			a = mergeSort(a);
			b = mergeSort(b);

		}
		sortedArray = merge(a,b);
		return sortedArray;
	}
	
	public static void main(String[] args){
		int[] a1 = {1,2,10,50,10,1,1,2,5,4,3,10};
		int[] a2 = {1,1,1};
		int[] a3 = {2,2,3,4,4,6};
		
		Project0 myProj = new Project0();
		//Test for merge.
		myProj.display(myProj.merge(a2,a3));
		//Test for mergeSort.
		myProj.display(myProj.mergeSort(a1));
	}
	
	public static void display(int a[]) {
	    int i;
	    for (i = 0; i < a.length; i++)
	        System.out.println(a[i]);
	    System.out.println();
	}  // display taken from lecture 3 Bubble.java file

}
