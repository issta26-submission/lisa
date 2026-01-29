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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON_bool is_str = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    str_item = cJSON_CreateString("sensor_id");

    // step 3: Configure - attach the number, string and a false flag to the root object
    add_ok_num = cJSON_AddItemToObject(root, "value", num_item);
    add_ok_str = cJSON_AddItemToObject(root, "id", str_item);
    false_item = cJSON_AddFalseToObject(root, "active");

    // step 4: Operate - fetch the items and check types
    fetched_num = cJSON_GetObjectItem(root, "value");
    fetched_str = cJSON_GetObjectItem(root, "id");
    is_str = cJSON_IsString(fetched_str);

    // step 5: Validate - serialize and compute a small score from results
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(add_ok_num) + (int)(add_ok_str) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(is_str);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}