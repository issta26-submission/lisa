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
//<ID> 2165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *got_flag = NULL;
    cJSON *got_age = NULL;
    cJSON *got_name = NULL;
    char *printed = NULL;
    const char *name_str = NULL;
    cJSON_bool added_flag = 0;
    cJSON_bool is_flag_true = 0;
    double age_val = 0.0;
    int validation = 0;

    // step 2: Setup (create object and add members)
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "age", 42.0);
    cJSON_AddStringToObject(root, "name", "Tester");
    flag = cJSON_CreateTrue();
    added_flag = cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate (serialize and retrieve members)
    printed = cJSON_PrintUnformatted(root);
    got_flag = cJSON_GetObjectItem(root, "flag");
    got_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(got_age);
    got_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(got_name);
    is_flag_true = cJSON_IsTrue(got_flag);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(is_flag_true != 0);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(age_val == 42.0);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(name_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}