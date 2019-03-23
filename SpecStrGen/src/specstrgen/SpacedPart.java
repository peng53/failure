/*
 * Copyright 2019 lm.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package specstrgen;

/**
 *
 * @author lm
 */
public class SpacedPart implements Part {
    public SpacedPart(Part child, boolean lalign, int length, char fillChar){
        this.wrapped = child;
        this.lalign = lalign;
        this.fillChar = fillChar;
        this.length = length;
    }
    @Override
    public String out() {
        StringBuilder sb = new StringBuilder();
        this.out(sb);
        return sb.toString();
    }
    @Override
    public void out(StringBuilder sb) {
        String wrappedStr = wrapped.out();
        int toFill = length-wrappedStr.length();
        if (lalign){
            sb.append(wrappedStr);
        }
        while (toFill-->0){
            sb.append(fillChar);
        }
        if (!lalign){
            sb.append(wrappedStr);
        }
    }
    boolean lalign;
    char fillChar;
    int length;
    Part wrapped;
}
