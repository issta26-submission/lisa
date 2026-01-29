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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddTrueToObject(cfg, "enabled");
    double dvals[3] = {10.5, 20.25, 30.75};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(cfg, "values", darr);
    const char *initial_note = "initial note";
    cJSON *note_ref = cJSON_CreateStringReference(initial_note);
    cJSON_AddItemToObject(cfg, "note", note_ref);

    // step 3: Operate
    char *prebuf = (char *)cJSON_malloc(512);
    memset(prebuf, 0, 512);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)pre_ok;
    cJSON *item0 = cJSON_GetArrayItem(darr, 0);
    cJSON *item1 = cJSON_GetArrayItem(darr, 1);
    double sum_first_two = cJSON_GetNumberValue(item0) + cJSON_GetNumberValue(item1);
    cJSON_AddNumberToObject(cfg, "sum_first_two", sum_first_two);
    cJSON *repl_note = cJSON_CreateStringReference("replaced note");
    cJSON_bool repl_ok = cJSON_ReplaceItemInObjectCaseSensitive(cfg, "note", repl_note);
    (void)repl_ok;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}