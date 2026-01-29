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
//<ID> 1160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *settings = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *flag_true = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *fetched_settings = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON *fetched_flag0 = (cJSON *)0;
    cJSON *fetched_flag1 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_flag0_bool = 0;
    cJSON_bool is_flag1_bool = 0;
    int validation_score = 0;

    // step 2: Setup - create root object, nested object and an array of booleans
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    flags = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "flags", flags);
    flag_true = cJSON_CreateTrue();
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddItemToArray(flags, flag_false);

    // step 3: Operate - retrieve nested objects and array elements using provided APIs
    fetched_settings = cJSON_GetObjectItem(root, "settings");
    fetched_flags = cJSON_GetObjectItem(fetched_settings, "flags");
    fetched_flag0 = cJSON_GetArrayItem(fetched_flags, 0);
    fetched_flag1 = cJSON_GetArrayItem(fetched_flags, 1);
    is_flag0_bool = cJSON_IsBool(fetched_flag0);
    is_flag1_bool = cJSON_IsBool(fetched_flag1);

    // step 4: Validate - compute a simple validation score based on retrievals and type checks
    validation_score = (int)(fetched_settings != (cJSON *)0) + (int)(fetched_flags != (cJSON *)0) + (int)is_flag0_bool + (int)is_flag1_bool;
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}