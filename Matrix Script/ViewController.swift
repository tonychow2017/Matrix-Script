//
//  ViewController.swift
//  Matrix Script
//
//  Created by Chun Wa Chow on 14/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

import UIKit
import WebKit

class ViewController: UIViewController, UITableViewDataSource {
    //MARK: Properties
    @IBOutlet weak var answerLabel: UILabel!
    @IBOutlet weak var mainTextView: UITextView!
    @IBOutlet weak var segmentedControl: UISegmentedControl!
    @IBOutlet weak var generalTable: UITableView!
    @IBOutlet weak var functionTable: UITableView!
    @IBOutlet weak var characterTable: UITableView!
    @IBOutlet weak var generalView: UIView!
    @IBOutlet weak var functionView: UIView!
    @IBOutlet weak var characterView: UIView!
    
    static let generalTableCells = [["docs","","Int","[","]",";"],["7","8","9","(",")",","],["4","5","6","\u{00D7}","\u{00F7}","\u{221A}"],["1","2","3","+","-","^"],["0",".","ANS","\u{21B2}","C","AC"]]
    static let functionTableCells = [["sin","cos","tan","asin","acos","atan"],["csc","sec","cot","acsc","asec","acot"],["log","ln","exp","sort","\u{2308}\u{2309}","\u{230A}\u{230B}"],["det","inv","rref","row","col","size"],["one","zero","Id","get","rep","~"],["\u{2211}","\u{220F}","A'","max","min","Mm"],["[]+[]","\u{222a}","\u{2229}","\u{0394}","{a}","[..]"]]
    static let characterTableCells = [["q","w","e","r","t","y","u","i","o","p"],["a","s","d","f","g","h","j","k","l"],["z","x","c","v","b","n","m","$"],["\u{03C0}","\u{03D5}","G","R","_","#","="]]

    static let screenWidth = Int(UIScreen.main.bounds.width)
    static let buttonColumnCount = generalTableCells[0].count
    static let smallButtonColumnCount = characterTableCells[0].count
    static let gap = UIDevice.current.userInterfaceIdiom == .phone ? 5 : 10
    static let buttonWidth = Int(min(70,((screenWidth-10)-gap)/buttonColumnCount-gap))
    static let smallButtonWidth = Int(min(50,((screenWidth-10)-gap)/smallButtonColumnCount-gap))
    static let firstIndent = (screenWidth-10-((buttonWidth+gap)*buttonColumnCount-gap))/2
    static let smallFirstIndent = (screenWidth-10-((smallButtonWidth+gap)*smallButtonColumnCount-gap))/2
    static let firaSans = loadFiraSans()
    static let buttonBackgroundImage = createBackgroundImageForButton(color: UIColor(red: 1, green: 0.647, blue: 0, alpha: 1))
    static var tableDataDict = [UITableView: [[String]]]()
    static var tableCellIdentifierDict = [UITableView: String]()
    
    static let mainStoryboard = UIStoryboard(name: "Main", bundle: nil)
    static var docController: UIViewController? = nil
    
    var anotherLabelString: String? = nil
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return ViewController.tableDataDict[tableView]!.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: ViewController.tableCellIdentifierDict[tableView]!, for: indexPath)
        for button in ViewController.createButtonSet(ViewController.tableDataDict[tableView]![indexPath.row], width: tableView == characterTable ? ViewController.smallButtonWidth : ViewController.buttonWidth, offset: tableView == characterTable ? ViewController.smallFirstIndent : ViewController.firstIndent) {
            if tableView == characterTable {
                button.frame = button.frame.offsetBy(dx: CGFloat(indexPath.row*(ViewController.gap+ViewController.smallButtonWidth)/2), dy: 0)
            }
            cell.addSubview(button)
        }
        cell.backgroundColor = UIColor.clear
       return cell
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        // constants
        ViewController.tableDataDict = [generalTable: ViewController.generalTableCells,
                                        functionTable: ViewController.functionTableCells,
                                        characterTable: ViewController.characterTableCells]
        ViewController.tableCellIdentifierDict = [generalTable: "general",
                                                  functionTable: "function",
                                                  characterTable: "character"]
        // UI
        for tableView in [generalTable!, functionTable!, characterTable!] {
            tableView.register(UITableViewCell.self, forCellReuseIdentifier: ViewController.tableCellIdentifierDict[tableView]!)
            tableView.dataSource = self
            tableView.separatorColor = UIColor.clear
            tableView.rowHeight = CGFloat(ViewController.gap+ViewController.buttonWidth)
            tableView.allowsSelection = false
        }
        segmentedControl.layer.cornerRadius = 5
        for i in 0..<segmentedControl.numberOfSegments {
            segmentedControl.setWidth(CGFloat((ViewController.screenWidth-10-ViewController.firstIndent*2)/segmentedControl.numberOfSegments), forSegmentAt: i)
        }
        let segmentedAttr = [NSAttributedStringKey.font: ViewController.firaSans.withSize(segmentedControl.frame.height*0.5)]
        segmentedControl.setTitleTextAttributes(segmentedAttr, for: .normal)
        segmentedControl.addTarget(nil, action: #selector(segmentTouched(_:)), for: .valueChanged)
        /*for family in UIFont.familyNames.sorted() {
            print(UIFont.fontNames(forFamilyName: family))
        }*/
        let dummy1 = UIView(frame: CGRect(x: 0, y: 0, width: 1, height: 0))
        mainTextView.tintColor = UIColor.black
        mainTextView.inputView = dummy1
        mainTextView.inputAssistantItem.leadingBarButtonGroups = []
        mainTextView.inputAssistantItem.trailingBarButtonGroups = []
        mainTextView.autocorrectionType = .no
        mainTextView.autocapitalizationType = .none
        answerLabel.isUserInteractionEnabled = true
        let recognizer = UITapGestureRecognizer(target: self, action: #selector(labelTapped(_:)))
        answerLabel.addGestureRecognizer(recognizer)
        //layout
        //let phoneWidth = 6*ViewController.buttonWidth + 5*ViewController.gap + 2*ViewController.firstIndent
        let phoneHeight = 5*ViewController.buttonWidth + 6*ViewController.gap + 1
        //print([phoneWidth, ViewController.screenWidth])
        //let ratio = CGFloat(Double(max(phoneWidth, ViewController.screenWidth))/Double(phoneHeight))
        let ratio = Double(ViewController.screenWidth-10)/Double(phoneHeight)
        let generalViewConstraint = NSLayoutConstraint(item: generalView, attribute: .width, relatedBy: .equal, toItem: generalView, attribute: .height, multiplier: CGFloat(ratio), constant: 1)
        generalViewConstraint.isActive = true
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
        reduceMemoryUsage()
    }
    
    static func loadFiraSans() -> UIFont {
        guard let font = UIFont(name: "FiraSans-Regular", size: UIFont.buttonFontSize) else {
            fatalError()
        }
        return font
    }
    
    static func createBackgroundImageForButton(color: UIColor) -> UIImage {
        UIGraphicsBeginImageContext(CGSize(width: buttonWidth, height: buttonWidth))
        color.setFill()
        UIRectFill(CGRect(x: 0, y: 0, width: buttonWidth, height: buttonWidth))
        let image = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext();
        return image!;
    }
    
    static func createButton(_ name: String) -> UIButton {
        let button = UIButton()
        button.setTitle(name, for: .normal);
        button.setTitleColor(UIColor.white, for: .normal)
        button.setBackgroundImage(buttonBackgroundImage, for: .highlighted)
        button.backgroundColor = UIColor.brown
        button.clipsToBounds = true
        button.addTarget(nil, action: #selector(buttonTouched(_:)), for: .touchDown)
        return button
    }
    
    static func createButtonSet(_ names: [String], width buttonWidth: Int, offset beforeOffset: Int) -> [UIButton] {
        var buttonSet = [UIButton]()
        var count = 0
        for name in names {
            let button = createButton(name);
            buttonSet += [button]
            button.frame = CGRect(x: (buttonWidth+ViewController.gap)*count + beforeOffset, y: ViewController.gap, width: buttonWidth, height: ViewController.buttonWidth)
            button.layer.cornerRadius = CGFloat(buttonWidth/2)
            button.titleLabel?.font = ViewController.firaSans.withSize(CGFloat(ViewController.buttonWidth*2/5))
            //button.titleLabel?.adjustsFontForContentSizeCategory = true
            count += 1
        }
        return buttonSet
    }
    
    static func insertableString(_ str: String) -> String {
        if str == "Int" {
            return "round"
        } else if str == "\u{21B2}" {
            return "\n"
        } else if str == "A\'" {
            return "transpose"
        } else if str == "Mm" {
            return "maxmin"
        } else if str == "\u{221A}" {
            return "sqrt"
        } else if str == "\u{00D7}" {
            return "*"
        } else if str == "\u{00F7}" {
            return "/"
        } else if str == "flat" {
            return "flatten"
        } else if str == "ANS" {
            return "$answer"
        } else if str == "\u{2211}" {
            return "sum"
        } else if str == "\u{220F}" {
            return "product"
        } else if str == "\u{222a}" {
            return "union"
        } else if str == "\u{2229}" {
            return "intersection"
        } else if str == "\u{0394}" {
            return "sym_diff"
        } else if str == "\u{2308}\u{2309}" {
            return "ceil"
        } else if str == "\u{230A}\u{230B}" {
            return "floor"
        } else if str == "[]+[]" {
            return "append"
        } else if str == "[..]" {
            return "resize"
        } else if str == "Id" {
            return "eye"
        } else if str == "\u{03C0}" {
            return "pi"
        } else if str == "\u{03D5}" {
            return "golden_ratio"
        } else if str == "G" {
            return "gravitational_const"
        } else if str == "R" {
            return "gas_const"
        } else {
            return str
        }
    }
    
    @objc func labelTapped(_ sender: UILabel) {
        //if let another = anotherLabelString {
        if (ViewController.docController == nil) {
            ViewController.docController = ViewController.mainStoryboard.instantiateViewController(withIdentifier:"DocumentationController")
            ViewController.docController!.loadViewIfNeeded()
        }
        if let another = anotherLabelString {
            if let controller = ViewController.docController as? WebViewController {
                controller.load(errorMessage: another);
            }
            self.present(ViewController.docController!, animated: true)
        } else if answerLabel.text! != "N/A" {
            if let controller = ViewController.docController as? WebViewController {
                controller.load(answer: answerLabel.text!)
            }
            self.present(ViewController.docController!, animated: true)
        }
    }
    
    @objc func buttonTouched(_ sender: UIButton) {
        if sender.currentTitle == "AC" {
            mainTextView.text = ""
        } else if (sender.currentTitle == "C" && mainTextView.text != "") {
            let range = mainTextView.selectedTextRange
            if range == nil {
                mainTextView.text = String(mainTextView.text.dropLast());
            } else if range!.isEmpty {
                let position = mainTextView.offset(from: mainTextView.beginningOfDocument, to: range!.start)
                if position == 0 {
                    if let next = mainTextView.position(from: mainTextView.beginningOfDocument, offset: 1) {
                        mainTextView.replace(mainTextView.textRange(from: mainTextView.beginningOfDocument, to: next)!, withText: "")
                    }
                } else {
                    if let before = mainTextView.position(from: range!.start, offset: -1) {
                        mainTextView.replace(mainTextView.textRange(from: before, to: range!.start)!, withText: "");
                    }
                    //let positionIndex = String.Index(encodedOffset: position);
                    //mainTextView.text.removeSubrange(mainTextView.text.index(before: positionIndex)..<positionIndex)
                }
            } else {
                mainTextView.replace(range!, withText: "")
            }
        } else if sender.currentTitle == "docs" {
            if (ViewController.docController == nil) {
                ViewController.docController = ViewController.mainStoryboard.instantiateViewController(withIdentifier: "DocumentationController")
                ViewController.docController!.loadViewIfNeeded()
            }
            if let controller = ViewController.docController as? WebViewController {
                controller.loadDocumentation()
            }
            self.present(ViewController.docController!, animated: true)
        } else {
            mainTextView.insertText(ViewController.insertableString(sender.currentTitle!))
        }
        (answerLabel.text!, anotherLabelString) = MatrixScript().calculate(input: mainTextView.text)
    }
    
    @objc func segmentTouched(_ sender: UISegmentedControl) {
        switch sender.selectedSegmentIndex {
        case 0:
            generalView.isHidden = false
            functionView.isHidden = true
            characterView.isHidden = true
        case 1:
            generalView.isHidden = true
            functionView.isHidden = false
            characterView.isHidden = true
        case 2:
            generalView.isHidden = true
            functionView.isHidden = true
            characterView.isHidden = false
        default:
            fatalError()
        }
    }
}

class WebViewController: UIViewController {
    @IBOutlet weak var docNavigationItem: UINavigationItem!
    @IBOutlet weak var docDoneButton: UIBarButtonItem!
    @IBOutlet weak var docWebView: WKWebView!
    
    static let htmlPrefix1 = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<title>"
    static let htmlPrefix2 = "</title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />\n</head>\n<body>"
    
    override func viewDidLoad() {
        let docButtonAttr = [NSAttributedStringKey.font: ViewController.firaSans, NSAttributedStringKey.foregroundColor: UIColor.brown]
        docDoneButton.setTitleTextAttributes(docButtonAttr, for: .normal)
        docDoneButton.action = #selector(docDoneButtonTouched(_:))
        //docWebView.configuration.dataDetectorTypes = .link
    }
    
    static func createLabel(text: String) -> UILabel {
        let label = UILabel()
        label.text = text
        label.font = UIFontMetrics.default.scaledFont(for: ViewController.firaSans)
        label.textColor = UIColor.brown
        label.adjustsFontForContentSizeCategory = true
        return label
    }
    
    func loadDocumentation() {
        docNavigationItem.titleView = WebViewController.createLabel(text: "Documentation")
        let url = Bundle.main.url(forResource: "docs", withExtension: "html")
        docWebView.load(URLRequest(url: url!))
    }
    
    func load(answer: String) {
        /*let answerLabel = UILabel();
        answerLabel.text = "Answer"
        answerLabel.font = UIFontMetrics.default.scaledFont(for: ViewController.firaSans)
        answerLabel.textColor = UIColor.brown
        answerLabel.adjustsFontForContentSizeCategory = true*/
        docNavigationItem.titleView = WebViewController.createLabel(text: "Answer")
        let htmlString = WebViewController.htmlPrefix1 + "Answer" + WebViewController.htmlPrefix2
        + "<h1>Answer</h1><h2>Full answer representation:</h2>\n<p>" + answer + ".</p></body></html>"
        //print(htmlString)
        docWebView.loadHTMLString(htmlString, baseURL: Bundle.main.url(forResource: "docs", withExtension: "html"))
    }
    
    func load(errorMessage: String) {
        docNavigationItem.titleView = WebViewController.createLabel(text: "Error")
        let htmlString = WebViewController.htmlPrefix1 + "Error" + WebViewController.htmlPrefix2
        + "<h1>Error</h1><h2>An error occured.</h2>\n<p>Error: " + errorMessage + "</p></body></html>"
        docWebView.loadHTMLString(htmlString, baseURL: Bundle.main.url(forResource: "docs", withExtension: "html"))
    }
    
    @objc func docDoneButtonTouched(_ sender: UIBarButtonItem) {
        //self.present(ViewController.mainStoryboard.instantiateViewController(withIdentifier: "MainController"), animated: true)
        ViewController.docController?.dismiss(animated: true)
    }
}


