package tsss_ed;

import javax.swing.*;

public class TSSS_Ed {
	private static void build_gui(){
		JFrame frame = new JFrame("???WEE");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		JLabel label = new JLabel("This th");
		frame.getContentPane().add(label);
		frame.pack();
		frame.setVisible(true);
	}

	public static void main(String[] args){
		javax.swing.SwingUtilites.invokeLater(new Runnable(){
			public void run(){
				build_gui();
			}
		});
	}
}
