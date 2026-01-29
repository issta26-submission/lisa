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
//<ID> 880
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool added_str = (cJSON_bool)0;
    char *printed = (char *)0;
    const char *extracted_str = (const char *)0;
    double extracted_num = 0.0;
    int validation_score = 0;

    // step 2: Configure - create object and items
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate - serialize and extract values
    printed = cJSON_PrintUnformatted(root);
    extracted_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    extracted_num = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));

    // step 4: Validate - simple aggregations of results
    validation_score = (int)(printed != (char *)0) + (int)(extracted_str != (const char *)0) + (int)(extracted_num == 42.0) + (int)(true_item != (cJSON *)0) + (int)added_str;
    (void)validation_score;

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}