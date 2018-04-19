//
//  ViewController.swift
//  Matrix Script
//
//  Created by Chun Wa Chow on 14/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITextViewDelegate {
    //MARK: Properties
    @IBOutlet weak var answerLabel: UILabel!
    @IBOutlet weak var mainTextView: UITextView!
    @IBOutlet weak var power2: UIButton!
    @IBOutlet weak var power3: UIButton!
    @IBOutlet weak var log: UIButton!
    @IBOutlet weak var zero: UIButton!
    @IBOutlet weak var one: UIButton!
    @IBOutlet weak var two: UIButton!
    @IBOutlet weak var three: UIButton!
    @IBOutlet weak var four: UIButton!
    @IBOutlet weak var five: UIButton!
    @IBOutlet weak var six: UIButton!
    @IBOutlet weak var seven: UIButton!
    @IBOutlet weak var eight: UIButton!
    @IBOutlet weak var nine: UIButton!
    @IBOutlet weak var dot: UIButton!
    @IBOutlet weak var power: UIButton!
    @IBOutlet weak var sum: UIButton!
    @IBOutlet weak var difference: UIButton!
    @IBOutlet weak var product: UIButton!
    @IBOutlet weak var quotient: UIButton!
    @IBOutlet weak var openrb: UIButton!
    @IBOutlet weak var closerb: UIButton!
    @IBOutlet weak var opensb: UIButton!
    @IBOutlet weak var closesb: UIButton!
    @IBOutlet weak var comma: UIButton!
    @IBOutlet weak var colon: UIButton!
    
    var buttonInsertString: [UIButton: String] = [:]
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        mainTextView.delegate = self
        buttonInsertString = [power2: "^2",
                              power3: "^3",
                              log: "log",
                              zero: "0",
                              one: "1",
                              two: "2",
                              three: "3",
                              four: "4",
                              five: "5",
                              six: "6",
                              seven: "7",
                              eight: "8",
                              nine: "9",
                              dot: ".",
                              power: "^",
                              sum: "+",
                              difference: "-",
                              product: "*",
                              quotient: "/",
                              openrb: "(",
                              closerb: ")",
                              opensb: "[",
                              closesb: "]",
                              comma: ",",
                              colon: ";",]
        let backgroundGreen = view.backgroundColor!
        for button in buttonInsertString.keys {
            button.layer.cornerRadius = button.frame.size.width/2
            button.layer.borderColor = backgroundGreen.cgColor
            button.layer.borderWidth = 2.5
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func calculateExpression(expression: UnsafeMutablePointer<Int8>) -> String {
        let resultString = UnsafeMutablePointer<Int8>(mutating: nil)
        let resultStringPointer = UnsafeMutablePointer<UnsafeMutablePointer<Int8>?>.allocate(capacity: 1)
        resultStringPointer.initialize(to: resultString)
        calculate_c(resultStringPointer, expression)
        return String(cString: resultStringPointer.pointee!)
    }
    
    //MARK: UITextViewDelegate
    //MARK: Actions

    @IBAction func buttonInput(_ sender: UIButton) {
        if let insertingString = buttonInsertString[sender] {
            mainTextView.text.append(insertingString)
            let input = mainTextView.text
            if let answerString = input?.withCString({ (ptr: UnsafePointer<Int8>) -> String in
                return calculateExpression(expression: UnsafeMutablePointer<Int8>(mutating: ptr))
            }) {
                answerLabel.text = answerString
            } else {
                answerLabel.text = "N/A"
            }
        }
    }
    
    func textViewDidChange(_ textView: UITextView) {
        let input = textView.text
        if let answerString = input?.withCString({ (ptr: UnsafePointer<Int8>) -> String in
            return calculateExpression(expression: UnsafeMutablePointer<Int8>(mutating: ptr))
        }) {
            answerLabel.text = answerString
        } else {
            answerLabel.text = "N/A"
        }
    }
}

