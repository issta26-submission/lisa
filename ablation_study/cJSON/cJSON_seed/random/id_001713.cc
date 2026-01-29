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
//<ID> 1713
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    cJSON *got_label_cs = (cJSON *)0;
    char *json_unformatted = (char *)0;
    double extracted_value = 0.0;
    char *extracted_label = (char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14);
    str_item = cJSON_CreateString("pi");

    // step 3: Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted(root);
    got_num = cJSON_GetObjectItem(root, "number");
    got_label_cs = cJSON_GetObjectItemCaseSensitive(root, "label");
    extracted_value = cJSON_GetNumberValue(got_num);
    extracted_label = cJSON_GetStringValue(got_label_cs);

    // step 5: Validate
    validation = (int)(extracted_value == 3.14) + (int)(extracted_label != (char *)0) + (int)(json_unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_item;
    (void)str_item;
    (void)got_num;
    (void)got_label_cs;
    (void)extracted_value;
    (void)extracted_label;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}