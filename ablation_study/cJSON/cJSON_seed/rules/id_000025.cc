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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(100.5);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *retr_value = NULL;
    cJSON *retr_flag = NULL;
    cJSON *computed = NULL;
    char *printed = NULL;
    double computed_value = 0.0;
    cJSON_bool added_value = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup
    added_value = cJSON_AddItemToObject(root, "value", num1);
    added_flag = cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate/Validate
    retr_value = cJSON_GetObjectItem(root, "value");
    replaced = cJSON_ReplaceItemViaPointer(root, retr_value, num2);
    retr_flag = cJSON_GetObjectItem(root, "flag");
    computed_value = 1.0 + (double)cJSON_IsTrue(retr_flag) * 9.0;
    computed = cJSON_CreateNumber(computed_value);
    cJSON_AddItemToObject(root, "computed", computed);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}