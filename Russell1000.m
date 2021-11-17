% read tickers
Russell=readtable('Russell_1000_component_stocks.csv','ReadVariableNames', false);
% length= number of tickers
length=height(Russell);
%length=10; for test
T=table;  %empty table
for i=1:length
    earnings=scrapeEarningsZacks(convertCharsToStrings(Russell{i,2})); % extract earnings
    if isempty(earnings)
        continue;       % if earnings does not exist, skip
    end
    
    for n=1:size(earnings,1)
        if ismember(earnings(n,2),{'Aug 2020', 'Sep 2020', 'Oct 2020'})
            T= [T; [Russell{i,2}, earnings(n,:)]];
        end
        % choose earnings with period ending Aug, Sep, Oct 2020 
        % to meet calendar quarter, stort in the table
        
    end
    
end
% remove duplicated company earnings
[C,ia]=unique(T.b1);
results=T(ia,:);
% data clean and re-organize
ans7=str2double(results{:,7});
ans6=str2double(results{:,6});
for x=1:height(results)
    results{x,7}={ans7(x)};
    results{x,6}={ans6(x)};
    results{x,4}=strrep(results{x,4},'$','');
    results{x,4}=strrep(results{x,4}, ' ', '');
    results{x,4}={str2double(results{x,4})};
    results{x,5}=strrep(results{x,5},'$','');
    results{x,5}=strrep(results{x,5}, ' ', '');
    results{x,5}={str2double(results{x,5})};
    results{x,2}=strrep(results{x,2}, '-20', '-2020');
    results{x,2}={datestr(results{x,2}, 'yyyy-mm-dd')};
end
% rename columns, and sort in ascending order in terms of 'Surprise%'
results.Properties.VariableNames={'Tickers','Date','Period ending','Estimate','Reported','Surprise','Surprise%'};
writetable(results, "Surprise.csv");