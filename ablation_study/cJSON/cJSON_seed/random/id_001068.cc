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
//<ID> 1068
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    char *printed = (char *)0;
    const char *str_value = (const char *)0;
    double num_value = 0.0;
    cJSON_bool add_ok_cs = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create an object, a number and a string item
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(123.0);
    strItem = cJSON_CreateString("hello cJSON");

    // step 3: Configure - attach items to the root using both CS and non-CS add APIs
    add_ok_cs = cJSON_AddItemToObjectCS(root, "num", num);
    add_ok = cJSON_AddItemToObject(root, "greeting", strItem);

    // step 4: Operate - fetch items, extract values, print and free the formatted JSON
    fetched_num = cJSON_GetObjectItem(root, "num");
    fetched_str = cJSON_GetObjectItem(root, "greeting");
    num_value = cJSON_GetNumberValue(fetched_num);
    str_value = cJSON_GetStringValue(fetched_str);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score from the results
    validation_score = (int)(root != (cJSON *)0) + (int)(num != (cJSON *)0) + (int)(strItem != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(num_value == 123.0) + (int)(str_value != (const char *)0) + (int)(add_ok_cs) + (int)(add_ok);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}