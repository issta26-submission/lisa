#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *metrics = (cJSON *)0;
    cJSON *score_item = (cJSON *)0;
    double score_value = 0.0;
    cJSON *new_arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string with nested object and number
    root = cJSON_Parse("{\"name\":\"device\",\"metrics\":{\"score\":42.5},\"values\":[1,2]}");

    // step 3: Configure - retrieve the nested metrics object and its numeric score
    metrics = cJSON_GetObjectItem(root, "metrics");
    score_item = cJSON_GetObjectItem(metrics, "score");
    score_value = cJSON_GetNumberValue(score_item);

    // step 4: Operate - create a new array, populate it with numbers derived from the score, and attach to root
    new_arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(score_value);
    num2 = cJSON_CreateNumber(score_value * 2.0);
    add_ok1 = cJSON_AddItemToArray(new_arr, num1);
    add_ok2 = cJSON_AddItemToArray(new_arr, num2);
    cJSON_AddItemToObject(root, "derived", new_arr);

    // step 5: Validate - serialize unformatted and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(metrics != (cJSON *)0) + (int)(score_item != (cJSON *)0) + (int)(score_value == 42.5) + (int)(add_ok1 != 0) + (int)(add_ok2 != 0) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete root which frees attached items
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}