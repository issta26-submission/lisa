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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_value = 0.0;
    cJSON_bool has_year = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(2025.0);
    str = cJSON_CreateString("example");
    meta = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "year", num);
    cJSON_AddItemToObject(root, "name", str);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "pi_hint", 3.14159);

    // step 4: Operate
    retrieved_value = cJSON_GetNumberValue(num);
    has_year = cJSON_HasObjectItem(root, "year");
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_Minify(printed);

    // step 5: Validate
    (void)retrieved_value;
    (void)has_year;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}