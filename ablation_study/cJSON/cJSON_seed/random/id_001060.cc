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
//<ID> 1060
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *label_text = "example";
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    char *printed = (char *)0;
    const char *retrieved_label = (const char *)0;
    double retrieved_number = 0.0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and two child items (number and string)
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.45);
    str_item = cJSON_CreateString(label_text);

    // step 3: Configure - add items to the root using both AddItemToObject and AddItemToObjectCS
    add_ok1 = cJSON_AddItemToObject(root, "value", num_item);
    add_ok2 = cJSON_AddItemToObjectCS(root, "label", str_item);

    // step 4: Operate - fetch the items back, read their values, and print the object unformatted
    fetched_num = cJSON_GetObjectItem(root, "value");
    fetched_str = cJSON_GetObjectItem(root, "label");
    retrieved_number = cJSON_GetNumberValue(fetched_num);
    retrieved_label = cJSON_GetStringValue(fetched_str);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score based on the results
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(add_ok1) + (int)(add_ok2) + (int)(retrieved_label != (const char *)0) + (int)(retrieved_number == 123.45);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}