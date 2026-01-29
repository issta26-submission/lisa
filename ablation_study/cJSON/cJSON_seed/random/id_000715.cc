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
//<ID> 715
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retr_str = (cJSON *)0;
    cJSON *retr_false = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_str = 0;
    cJSON_bool is_false = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("example");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_str = cJSON_AddItemToObject(root, "message", str_item);
    added_false = cJSON_AddItemToObject(root, "active", false_item);

    // step 4: Operate
    retr_str = cJSON_GetObjectItem(root, "message");
    retr_false = cJSON_GetObjectItem(root, "active");
    printed = cJSON_PrintUnformatted(root);
    is_str = cJSON_IsString(retr_str);
    is_false = cJSON_IsFalse(retr_false);

    // step 5: Validate
    presence_count = (printed != (char *)0) + (int)added_str + (int)added_false + (int)is_str + (int)is_false;
    (void)presence_count;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}