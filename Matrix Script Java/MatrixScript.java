import java.awt.*;
import javax.swing.*;

public class MatrixScript {
    public static void main(String[] args) {
        System.loadLibrary("Calc");
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                MainFrame.createAndShowGUI();
            }
        });
    }
    
    public static native String calculateString(String expression);
}
