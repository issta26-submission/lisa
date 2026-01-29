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
//<ID> 576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(entry, "name", str_item);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(entry, "value", num_item);
    cJSON *flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(entry, "flag", flag_item);

    // step 2: Configure
    char *updated = cJSON_SetValuestring(str_item, "updated");
    double newnum = cJSON_SetNumberHelper(num_item, 6.28);
    (void)updated;
    (void)newnum;

    // step 3: Operate and Validate
    cJSON *got_entry = cJSON_GetObjectItem(root, "entry");
    cJSON *got_name = cJSON_GetObjectItem(got_entry, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_value = cJSON_GetObjectItem(got_entry, "value");
    double num_val = cJSON_GetNumberValue(got_value);
    cJSON *got_flag = cJSON_GetObjectItem(got_entry, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    double computed = num_val + (double)flag_is_false;
    cJSON_AddNumberToObject(root, "computed", computed);
    cJSON_AddStringToObject(root, "final_name", name_val);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}