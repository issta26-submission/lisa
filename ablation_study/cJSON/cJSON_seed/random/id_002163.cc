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
//<ID> 2163
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
    cJSON *t = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *got_flag = NULL;
    cJSON *got_age = NULL;
    cJSON *got_name = NULL;
    char *printed = NULL;
    double age_val = 0.0;
    const char *name_val = NULL;
    cJSON_bool is_t = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    str = cJSON_CreateString("Eve");
    t = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "age", num);
    cJSON_AddItemToObject(root, "name", str);
    cJSON_AddItemToObject(root, "flag", t);

    // step 3: Operate
    printed = cJSON_PrintUnformatted(root);
    got_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(got_age);
    got_name = cJSON_GetObjectItem(root, "name");
    name_val = cJSON_GetStringValue(got_name);
    got_flag = cJSON_GetObjectItem(root, "flag");
    is_t = cJSON_IsTrue(got_flag);
    buf[0] = (char)(is_t + '0');

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(str != NULL);
    validation ^= (int)(t != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(age_val == 42.0);
    validation ^= (int)(name_val != NULL);
    validation ^= (int)(is_t != 0);
    buf[1] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}