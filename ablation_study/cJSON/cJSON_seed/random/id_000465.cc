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
//<ID> 465
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
    cJSON *parsed = (cJSON *)0;
    char *printed = (char *)0;
    const cJSON *pi_item = (const cJSON *)0;
    double nval = 0.0;
    cJSON_bool has_label = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);
    str_item = cJSON_CreateString("example label");

    // step 3: Configure
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    pi_item = cJSON_GetObjectItem(parsed, "pi");
    nval = cJSON_GetNumberValue(pi_item);
    has_label = cJSON_HasObjectItem(parsed, "label");

    // step 5: Validate
    (void)nval;
    (void)has_label;
    (void)pi_item;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}