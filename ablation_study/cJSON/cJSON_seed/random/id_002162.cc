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
//<ID> 2162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[48];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *got_age = NULL;
    cJSON *got_name = NULL;
    cJSON *got_active = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    const char *name_str = NULL;
    double age_val = 0.0;
    cJSON_bool added_active = 0;
    cJSON_bool is_active = 0;
    int validation = 0;

    // step 2: Setup (create object and add members, including a true item)
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "age", 42.0);
    cJSON_AddStringToObject(root, "name", "Bob");
    true_item = cJSON_CreateTrue();
    added_active = cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Operate (serialize and retrieve object members)
    printed = cJSON_PrintUnformatted(root);
    got_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(got_age);
    got_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(got_name);
    got_active = cJSON_GetObjectItem(root, "active");
    is_active = cJSON_IsTrue(got_active);
    buf[0] = (char)(is_active + '0');

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(got_active != NULL);
    validation ^= (int)(age_val == 42.0);
    validation ^= (int)(name_str != NULL);
    validation ^= (int)(added_active != 0);
    validation ^= (int)(is_active != 0);
    buf[1] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}