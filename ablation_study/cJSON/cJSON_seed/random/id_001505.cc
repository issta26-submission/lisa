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
//<ID> 1505
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
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_num = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("alpha_value");
    num_item = cJSON_CreateNumber(123.0);

    // step 3: Configure
    added_str = cJSON_AddItemToObjectCS(root, "alpha", str_item);
    added_num = cJSON_AddItemToObjectCS(root, "beta", num_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_num = cJSON_GetObjectItem(root, "beta");

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_str + (int)added_num + (int)(retrieved_num != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_DeleteItemFromObject(root, "alpha");
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_num;
    // API sequence test completed successfully
    return 66;
}