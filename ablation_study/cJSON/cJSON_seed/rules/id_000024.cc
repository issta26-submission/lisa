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
//<ID> 24
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
    cJSON *num2 = cJSON_CreateNumber(84.0);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *computed_item = NULL;
    cJSON *retrieved_n = NULL;
    char *printed = NULL;
    double replaced_val = 0.0;
    int is_true_flag = 0;
    double computed = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "n", num1);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, num1, num2);
    retrieved_n = cJSON_GetObjectItem(root, "n");
    replaced_val = cJSON_GetNumberValue(retrieved_n);
    is_true_flag = (int)cJSON_IsTrue(true_item);
    computed = replaced_val + (double)is_true_flag;
    computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}