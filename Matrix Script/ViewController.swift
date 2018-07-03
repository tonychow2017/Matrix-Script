//
//  ViewController.swift
//  Matrix Script
//
//  Created by Chun Wa Chow on 14/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

import UIKit

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
    
    
    static let generalTableCells = [["(",")","[","]",",",";"],["7","8","9","^","",""],["4","5","6","*","/",""],["1","2","3","+","-","docs"],["0",".","","\\n","C","AC"]]
    static let functionTableCells = [["sin","cos","tan","asin","acos","atan"],["csc","sec","cot","acsc","asec","acot"],["log","ln","exp","ceil","floor","rnd"],["det","inv","rref","A\'","sum","prod"]]
    static let characterTableCells = [["$"]]
    //static let tableDataDict = [generalTable: generalTableCells]
    static let screenWidth = Int(UIScreen.main.bounds.width)
    static let buttonColumnCount = generalTableCells[0].count
    static let gap = UIDevice.current.userInterfaceIdiom == .phone ? 5 : 10
    static let buttonWidth = Int(min(70,((screenWidth-gap*2)-gap)/buttonColumnCount-gap))
    static let firstIndent = (screenWidth-2*gap-((buttonWidth+gap)*buttonColumnCount-gap))/2
    static let firaSans = loadFiraSans()
    static var tableDataDict = [UITableView: [[String]]]()
    static var tableCellIdentifierDict = [UITableView: String]()
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return ViewController.tableDataDict[tableView]!.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: ViewController.tableCellIdentifierDict[tableView]!, for: indexPath)
        for button in createButtonSet(ViewController.tableDataDict[tableView]![indexPath.row]) {
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
        //generalTable.dataSource = self
        //generalTable.register(UITableViewCell.self, forCellReuseIdentifier: "generalTableCell")
        segmentedControl.layer.cornerRadius = 5
        for i in 0..<segmentedControl.numberOfSegments {
            segmentedControl.setWidth(CGFloat((ViewController.screenWidth-ViewController.gap*2-ViewController.firstIndent*2)/segmentedControl.numberOfSegments), forSegmentAt: i)
        }
        let attr = [NSAttributedStringKey.font: ViewController.firaSans.withSize(segmentedControl.frame.height*0.5)]
        segmentedControl.setTitleTextAttributes(attr, for: .normal)
        segmentedControl.addTarget(nil, action: #selector(segmentTouched(_:)), for: .valueChanged)
        /*for family in UIFont.familyNames.sorted() {
            print(UIFont.fontNames(forFamilyName: family))
        }*/
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    static func loadFiraSans() -> UIFont {
        guard let font = UIFont(name: "FiraSans-Regular", size: UIFont.buttonFontSize) else {
            fatalError()
        }
        return font
    }
    
    func createButton(_ name: String) -> UIButton {
        let button = UIButton()
        button.setTitle(name, for: .normal);
        button.setTitleColor(UIColor.white, for: .normal)
        button.backgroundColor = UIColor.brown
        button.addTarget(nil, action: #selector(buttonTouched(_:)), for: .touchDown)
        return button
    }
    
    func createButtonSet(_ names: [String]) -> [UIButton] {
        var buttonSet = [UIButton]()
        var count = 0
        for name in names {
            let button = createButton(name);
            buttonSet += [button]
            button.frame = CGRect(x: (ViewController.buttonWidth+ViewController.gap)*count+ViewController.firstIndent, y: ViewController.gap, width: ViewController.buttonWidth, height: ViewController.buttonWidth)
            button.layer.cornerRadius = CGFloat(ViewController.buttonWidth/2)
            button.titleLabel?.font = ViewController.firaSans.withSize(CGFloat(ViewController.buttonWidth*2/5))
            //button.titleLabel?.adjustsFontForContentSizeCategory = true
            count += 1
        }
        return buttonSet
    }
    
    static func insertableString(_ str: String) -> String {
        if str == "rnd" {
            return "round"
        } else if str == "\\n" {
            return "\n"
        } else if str == "A\'" {
            return "transpose"
        } else {
            return str
        }
    }
    
    @objc func buttonTouched(_ sender: UIButton) {
        if (sender.currentTitle == "AC") {
            mainTextView.text = ""
        } else if (sender.currentTitle == "C") {
            mainTextView.text = String(mainTextView.text.dropLast())
        } else {
            mainTextView.text? += ViewController.insertableString(sender.currentTitle!)
        }
        answerLabel.text = MatrixScript().calculate(input: mainTextView.text)
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


