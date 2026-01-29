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
//<ID> 1284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *null_item = NULL;
    cJSON *num_ref_item = NULL;
    cJSON *sum_item = NULL;
    cJSON *tmp = NULL;
    char *printed = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double vref = 0.0;
    double sum = 0.0;
    cJSON_bool add_ref_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.5);
    cJSON_AddItemToObject(root, "num", num_item);
    cJSON_AddNumberToObject(root, "direct", 2.25);
    null_item = cJSON_AddNullToObject(root, "nothing");
    add_ref_ok = cJSON_AddItemReferenceToObject(root, "num_ref", num_item);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "num");
    v1 = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "direct");
    v2 = cJSON_GetNumberValue(tmp);
    sum = v1 + v2;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    num_ref_item = cJSON_GetObjectItem(root, "num_ref");
    vref = cJSON_GetNumberValue(num_ref_item);
    (void)add_ref_ok;
    (void)vref;

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}