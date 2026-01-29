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
//<ID> 1509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item_a = (cJSON *)0;
    cJSON *item_b = (cJSON *)0;
    cJSON *retrieved_a = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    int validation_score = 0;
    const char *value_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_a = cJSON_CreateString("alpha");
    item_b = cJSON_CreateString("beta");

    // step 3: Configure
    added_a = cJSON_AddItemToObjectCS(root, "A", item_a);
    added_b = cJSON_AddItemToObjectCS(root, "B", item_b);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_a = cJSON_GetObjectItem(root, "A");
    value_str = cJSON_GetStringValue(retrieved_a);
    cJSON_DeleteItemFromObject(root, "B");

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_a + (int)added_b + (int)(retrieved_a != (cJSON *)0) + (int)(value_str != (const char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)value_str;
    (void)retrieved_a;
    // API sequence test completed successfully
    return 66;
}