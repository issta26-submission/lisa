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
//<ID> 1318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *stats = (cJSON *)0;
    cJSON *stats_obj = (cJSON *)0;
    cJSON *mean_item = (cJSON *)0;
    char *printed = (char *)0;
    double mean_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, two numbers and a stats object
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.00);
    stats = cJSON_CreateObject();
    cJSON_AddNumberToObject(stats, "mean", 2.57);

    // step 3: Configure - populate the array and attach items/objects to the root
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "measurements", arr);
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddNumberToObject(root, "constant", 7.0);

    // step 4: Operate - retrieve the stats object and the mean item, then extract its numeric value
    stats_obj = cJSON_GetObjectItem(root, "stats");
    mean_item = cJSON_GetObjectItem(stats_obj, "mean");
    mean_value = cJSON_GetNumberValue(mean_item);

    // step 5: Serialize and validate - produce an unformatted JSON string and compute a simple score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(mean_item != (cJSON *)0) + (int)(mean_value == 2.57);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which frees attached items)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}