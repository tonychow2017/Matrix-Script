//
//  ViewController.swift
//  Matrix Script
//
//  Created by Chun Wa Chow on 14/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDataSource, UITextViewDelegate {

    //MARK: Properties
    @IBOutlet weak var answerLabel: UILabel!
    @IBOutlet weak var mainTextView: UITextView!
    @IBOutlet weak var segmentedControl: UISegmentedControl!
    @IBOutlet weak var generalTable: UITableView!
    
    static var generalTableCells = ["+","-","*","/","^"]
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return ViewController.generalTableCells.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = generalTable.dequeueReusableCell(withIdentifier: "generalTableCell", for: indexPath)
        //cell.textLabel!.text = ViewController.generalTableCells[indexPath.row]
        cell.addSubview(createButton(ViewController.generalTableCells[indexPath.row]))
        cell.backgroundColor = UIColor.clear
        //cell.textLabel!.backgroundColor = UIColor.clear
        //cell.selectedBackgroundView = cell.backgroundView//UIView()
        //cell.selectedBackgroundView?.isOpaque = false
        //cell.isOpaque = false
        //cell.textLabel!.isOpaque = false
        return cell
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        mainTextView.delegate = self
        generalTable.register(UITableViewCell.self, forCellReuseIdentifier: "generalTableCell")
        //generalTable.register(UITableViewCell.self,"generalTableCell")
        generalTable.dataSource = self
        generalTable.separatorColor = UIColor.clear
        generalTable.rowHeight = 60
        generalTable.allowsSelection = false
        segmentedControl.layer.cornerRadius = 5
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func textViewDidChange(_ textView: UITextView) {
        answerLabel.text = MatrixScript().calculate(input: textView.text);
    }
    
    func createButton(_ name: String) -> UIButton {
        let button = UIButton()
        button.setTitle(name, for: .normal);
        button.setTitleColor(UIColor.white, for: .normal)
        button.backgroundColor = UIColor.brown
        button.layer.cornerRadius = 25
        button.frame = CGRect(x: 5, y: 5, width: 50, height: 50)
        button.addTarget(nil,action: #selector(buttonTouched(_:)), for: .touchDown)
        return button
    }
    
    @objc func buttonTouched(_ sender: UIButton) {
        mainTextView.text? += sender.currentTitle!
    }
}

