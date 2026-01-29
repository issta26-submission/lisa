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
//<ID> 714
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retr_name = (cJSON *)0;
    cJSON *retr_flag = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool is_name_string = 0;
    cJSON_bool is_flag_false = 0;
    int validate_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");
    flag_item = cJSON_CreateFalse();

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_flag = cJSON_AddItemToObject(root, "flag", flag_item);

    // step 4: Operate
    retr_name = cJSON_GetObjectItem(root, "name");
    retr_flag = cJSON_GetObjectItem(root, "flag");
    is_name_string = cJSON_IsString(retr_name);
    is_flag_false = cJSON_IsFalse(retr_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validate_score = (int)added_name + (int)added_flag + (int)is_name_string + (int)is_flag_false + (printed != (char *)0);
    (void)validate_score;
    (void)retr_name;
    (void)retr_flag;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}