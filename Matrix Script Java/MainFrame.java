import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.text.*;

public class MainFrame extends JFrame {
    private static final Color BACKGROUND_GREEN = Color.GREEN.darker().darker();
    public static void createAndShowGUI() {
        new MainFrame().setVisible(true);
    }
    
    MainFrame() {
        super("Matrix Script");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setLayout(new BorderLayout());
        this.add(new MainPanel(), BorderLayout.CENTER);
        this.pack();
        this.setLocationRelativeTo(null);
    }
    
    static class MainPanel extends JPanel {
        private JTextArea textArea = new JTextArea(8,30);
        private JLabel answer = new JLabel("N/A", SwingConstants.CENTER);
        MainPanel() {
            super(new BorderLayout());
            this.setBackground(BACKGROUND_GREEN);
            textArea.setFont(textArea.getFont().deriveFont(25f));
            textArea.setBackground(Color.YELLOW);
            textArea.setForeground(Color.ORANGE.darker().darker());
            textArea.setBorder(new LineBorder(BACKGROUND_GREEN,5));
            answer.setFont(answer.getFont().deriveFont(25f));
            answer.setForeground(Color.WHITE);
            answer.setOpaque(false);
            this.add(textArea, BorderLayout.CENTER);
            this.add(answer, BorderLayout.PAGE_END);
            textArea.getDocument().addDocumentListener(new DocumentListener() {
                @Override
                public void insertUpdate(DocumentEvent ev) {
                    calculate(ev);
                }
                
                @Override
                public void changedUpdate(DocumentEvent ev) {
                    calculate(ev);
                }
                
                @Override
                public void removeUpdate(DocumentEvent ev) {
                    calculate(ev);
                }
                
                void calculate(DocumentEvent ev) {
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                Document doc = ev.getDocument();
                                String expression = doc.getText(0,doc.getLength());
                                answer.setText(MatrixScript.calculateString(expression));
                            } catch (Exception ex) {
                            }
                        }
                    });
                }
            });
        }
    }
}
