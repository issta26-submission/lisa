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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations/Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(100.5);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *sum_item = NULL;
    cJSON *retrieved = NULL;
    double val1 = 0.0;
    double val2 = 0.0;
    double computed = 0.0;
    char *printed = NULL;

    // step 2: Setup/Configure
    cJSON_AddItemToObject(root, "first", num1);
    cJSON_AddItemToObject(root, "second", num2);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate/Validate
    retrieved = cJSON_GetObjectItem(root, "first");
    val1 = cJSON_GetNumberValue(retrieved);
    retrieved = cJSON_GetObjectItem(root, "second");
    val2 = cJSON_GetNumberValue(retrieved);
    computed = val1 + val2;
    sum_item = cJSON_CreateNumber(computed);
    cJSON_ReplaceItemViaPointer(root, num1, sum_item);
    computed += (double)cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    cJSON_AddItemToObject(root, "computed", cJSON_CreateNumber(computed));

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}