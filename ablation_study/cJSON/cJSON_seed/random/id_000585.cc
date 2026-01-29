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
//<ID> 585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "{\"values\":[1.5,2.5],\"meta\":{\"scale\":2.0}}";
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *newnum = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *scale_item = (cJSON *)0;
    double second_value = 0.0;
    double scale_value = 0.0;
    cJSON_bool add_ok = 0;
    cJSON_bool require_null_terminated = 1;

    // step 2: Initialize (parse input)
    root = cJSON_ParseWithOpts(json, &parse_end, require_null_terminated);

    // step 3: Configure (locate objects and prepare new item)
    values = cJSON_GetObjectItem(root, "values");
    meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    newnum = cJSON_CreateNumber(3.5);
    add_ok = cJSON_AddItemToArray(values, newnum);

    // step 4: Operate (retrieve items and numeric values)
    second_item = cJSON_GetArrayItem(values, 1);
    second_value = cJSON_GetNumberValue(second_item);
    scale_item = cJSON_GetObjectItem(meta, "scale");
    scale_value = cJSON_GetNumberValue(scale_item);

    // step 5: Validate (use values to avoid compiler warnings)
    (void)add_ok;
    (void)parse_end;
    (void)second_value;
    (void)scale_value;
    (void)newnum;
    (void)values;
    (void)meta;
    (void)second_item;
    (void)scale_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}