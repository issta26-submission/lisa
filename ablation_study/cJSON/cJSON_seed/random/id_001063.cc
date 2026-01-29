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
//<ID> 1063
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *count = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *fetched_meta = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    cJSON *fetched_count = (cJSON *)0;
    char *printed = (char *)0;
    char *label_value = (char *)0;
    double count_value = 0.0;
    cJSON_bool add_ok_root_meta = (cJSON_bool)0;
    cJSON_bool add_ok_root_count = (cJSON_bool)0;
    cJSON_bool add_ok_meta_label = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create objects and primitive items
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    count = cJSON_CreateNumber(42.0);
    label = cJSON_CreateString("answer");

    // step 3: Configure - attach items using both AddItemToObjectCS and AddItemToObject
    add_ok_root_meta = cJSON_AddItemToObjectCS(root, "meta", meta);
    add_ok_root_count = cJSON_AddItemToObject(root, "count", count);
    add_ok_meta_label = cJSON_AddItemToObjectCS(meta, "label", label);

    // step 4: Operate - fetch items, read string and numeric values, print JSON
    fetched_meta = cJSON_GetObjectItem(root, "meta");
    fetched_label = cJSON_GetObjectItem(fetched_meta, "label");
    fetched_count = cJSON_GetObjectItem(root, "count");
    label_value = cJSON_GetStringValue(fetched_label);
    count_value = cJSON_GetNumberValue(fetched_count);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score from the results
    validation_score = (int)(root != (cJSON *)0) + (int)(meta != (cJSON *)0) + (int)(count != (cJSON *)0) + (int)(label != (cJSON *)0) + (int)(add_ok_root_meta) + (int)(add_ok_root_count) + (int)(add_ok_meta_label) + (int)(fetched_meta != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(fetched_count != (cJSON *)0) + (int)(label_value != (char *)0) + (int)(count_value == 42.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}