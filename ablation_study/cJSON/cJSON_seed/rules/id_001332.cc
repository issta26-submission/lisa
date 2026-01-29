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
//<ID> 1332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"metrics\": {\"val\": 42.5, \"label\": \"sensor\"}}";
    cJSON *root = NULL;
    cJSON *metrics = NULL;
    cJSON *val_item = NULL;
    cJSON *val_plus_item = NULL;
    cJSON *new_num = NULL;
    cJSON *new_str = NULL;
    cJSON *dup_metrics = NULL;
    char *rendered = NULL;
    double val = 0.0;
    double val_plus = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    metrics = cJSON_GetObjectItem(root, "metrics");
    val_item = cJSON_GetObjectItem(metrics, "val");
    val = cJSON_GetNumberValue(val_item);
    new_num = cJSON_CreateNumber(val + 10.0);
    new_str = cJSON_CreateString("added_label");
    cJSON_AddItemToObject(root, "val_plus", new_num);
    cJSON_AddItemToObject(root, "new_label", new_str);

    // step 3: Operate / Validate
    val_plus_item = cJSON_GetObjectItem(root, "val_plus");
    val_plus = cJSON_GetNumberValue(val_plus_item);
    dup_metrics = cJSON_Duplicate(metrics, 1);
    rendered = cJSON_PrintUnformatted(root);
    cJSON_free(rendered);
    (void)val;
    (void)val_plus;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_metrics);
    // API sequence test completed successfully
    return 66;
}