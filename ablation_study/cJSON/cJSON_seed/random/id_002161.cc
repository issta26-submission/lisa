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
//<ID> 2161
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
    cJSON *got_num = NULL;
    cJSON *got_str = NULL;
    char *printed = NULL;
    cJSON_bool added_flag = 0;
    double pi_val = 0.0;
    const char *msg_val = NULL;
    cJSON_bool is_flag_true = 0;
    int validation = 0;

    // step 2: Setup (create object and add members)
    root = cJSON_CreateObject();
    t = cJSON_CreateTrue();
    added_flag = cJSON_AddItemToObject(root, "flag", t);
    num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "pi", num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "msg", str);

    // step 3: Operate (serialize and retrieve object items)
    printed = cJSON_PrintUnformatted(root);
    got_flag = cJSON_GetObjectItem(root, "flag");
    got_num = cJSON_GetObjectItem(root, "pi");
    got_str = cJSON_GetObjectItem(root, "msg");
    pi_val = cJSON_GetNumberValue(got_num);
    msg_val = cJSON_GetStringValue(got_str);
    is_flag_true = cJSON_IsTrue(got_flag);

    // step 4: Validate (accumulate simple checks into validation)
    validation ^= (int)(root != NULL);
    validation ^= (int)(t != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(str != NULL);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(pi_val == 3.14);
    validation ^= (int)(msg_val != NULL);
    validation ^= (int)(is_flag_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}