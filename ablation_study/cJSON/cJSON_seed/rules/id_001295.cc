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
//<ID> 1295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *true_item = NULL;
    cJSON *detached = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    double v1 = 0.0;
    double v_det = 0.0;
    double total = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(123.45);
    cJSON_AddItemToObject(root, "num1", num1);
    num2 = cJSON_CreateNumber(67.89);
    cJSON_AddItemToObject(root, "num2", num2);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "explicit_true", true_item);
    cJSON_AddTrueToObject(root, "direct_true");

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, "num1");
    v1 = cJSON_GetNumberValue(got);
    detached = cJSON_DetachItemFromObject(root, "num2");
    v_det = cJSON_GetNumberValue(detached);
    total = v1 + v_det;
    cJSON_AddItemToObject(root, "num2_readded", detached);
    got = cJSON_GetObjectItem(root, "direct_true");
    (void)cJSON_IsTrue(got);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}