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
//<ID> 1065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *numItem = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *fetched_str_item = (cJSON *)0;
    cJSON *fetched_nested = (cJSON *)0;
    cJSON *fetched_num_item = (cJSON *)0;
    char *printed = (char *)0;
    char *got_str = (char *)0;
    double got_num = 0.0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    cJSON_bool add_ok_status = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, a nested object, a number and a string
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    numItem = cJSON_CreateNumber(42.0);
    strItem = cJSON_CreateString("ok");

    // step 3: Configure - attach number into nested using cJSON_AddItemToObject, attach nested and status into root using both add APIs
    add_ok_num = cJSON_AddItemToObject(nested, "count", numItem);
    add_ok_status = cJSON_AddItemToObjectCS(root, "status", strItem);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 4: Operate - fetch items, read values and obtain compact JSON text
    fetched_str_item = cJSON_GetObjectItem(root, "status");
    got_str = cJSON_GetStringValue(fetched_str_item);
    fetched_nested = cJSON_GetObjectItem(root, "nested");
    fetched_num_item = cJSON_GetObjectItem(fetched_nested, "count");
    got_num = cJSON_GetNumberValue(fetched_num_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute a small validation score from the results
    validation_score = (int)(root != (cJSON *)0) + (int)(nested != (cJSON *)0) + (int)(add_ok_num) + (int)(add_ok_status) + (int)(got_str != (char *)0) + (int)(got_num == 42.0) + (int)(printed != (char *)0);
    (void)validation_score;
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}