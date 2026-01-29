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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls:
    // creation, adding items, arrays, nested objects, printing, parsing,
    // duplicating, detaching/replacing items and cleanup.
    //
    // Returns 66 on success, negative value on failure.

    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return -1;
    }

    // Create and add a string item ("name": "cJSON")
    cJSON *name = cJSON_CreateString("cJSON");
    if (!name) {
        cJSON_Delete(root);
        return -2;
    }
    cJSON_AddItemToObject(root, "name", name); // root now owns 'name'

    // Create and add a number item ("version": 1.7)
    cJSON *version = cJSON_CreateNumber(1.7);
    if (!version) {
        cJSON_Delete(root);
        return -3;
    }
    cJSON_AddItemToObject(root, "version", version);

    // Create an array ("features": [...])
    cJSON *features = cJSON_CreateArray();
    if (!features) {
        cJSON_Delete(root);
        return -4;
    }
    cJSON_AddItemToObject(root, "features", features);

    // Add items to array
    cJSON *f1 = cJSON_CreateString("parsing");
    cJSON *f2 = cJSON_CreateString("printing");
    cJSON *f3 = cJSON_CreateString("validation");
    if (!f1 || !f2 || !f3) {
        // any created items will be cleaned up when deleting root
        cJSON_Delete(root);
        return -5;
    }
    cJSON_AddItemToArray(features, f1);
    cJSON_AddItemToArray(features, f2);
    cJSON_AddItemToArray(features, f3);

    // Add a boolean and a null at top level
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();
    if (!true_item || !null_item) {
        cJSON_Delete(root);
        return -6;
    }
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToObject(root, "nothing", null_item);

    // Create a nested object "meta": { "build": 2026, "author": "open" }
    cJSON *meta = cJSON_CreateObject();
    if (!meta) {
        cJSON_Delete(root);
        return -7;
    }
    cJSON *build = cJSON_CreateNumber(2026);
    cJSON *author = cJSON_CreateString("open");
    if (!build || !author) {
        cJSON_Delete(root);
        return -8;
    }
    cJSON_AddItemToObject(meta, "build", build);
    cJSON_AddItemToObject(meta, "author", author);
    cJSON_AddItemToObject(root, "meta", meta);

    // Print the JSON to a string (unformatted) and parse it back
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) {
        cJSON_Delete(root);
        return -9;
    }

    cJSON *parsed = cJSON_Parse(printed);
    // free printed representation
#ifdef cJSON_free
    cJSON_free(printed);
#else
    free(printed);
#endif
    if (!parsed) {
        cJSON_Delete(root);
        return -10;
    }

    // Validate some parsed contents:
    cJSON *p_name = cJSON_GetObjectItem(parsed, "name");
    cJSON *p_version = cJSON_GetObjectItem(parsed, "version");
    cJSON *p_features = cJSON_GetObjectItem(parsed, "features");
    cJSON *p_meta = cJSON_GetObjectItem(parsed, "meta");

    if (!p_name || !cJSON_IsString(p_name) || !p_name->valuestring) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -11;
    }
    if (!p_version || !cJSON_IsNumber(p_version)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -12;
    }
    // approximate check for 1.7
    if (!(p_version->valuedouble > 1.6999 && p_version->valuedouble < 1.7001)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -13;
    }
    if (!p_features || !cJSON_IsArray(p_features)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -14;
    }
    if ((size_t)cJSON_GetArraySize(p_features) != 3) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -15;
    }
    if (!p_meta || !cJSON_IsObject(p_meta)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -16;
    }
    cJSON *p_build = cJSON_GetObjectItem(p_meta, "build");
    if (!p_build || !cJSON_IsNumber(p_build) || (int)p_build->valuedouble != 2026) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -17;
    }

    // Duplicate parsed (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    if (!dup) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -18;
    }

    // Detach the "meta" object from duplicate (takes ownership of the detached item)
    cJSON *detached_meta = cJSON_DetachItemFromObject(dup, "meta");
    if (!detached_meta) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        return -19;
    }

    // Replace the "name" in duplicate with a new string
    cJSON *new_name = cJSON_CreateString("libcJSON");
    if (!new_name) {
        // re-attach detached_meta to avoid losing it when deleting dup
        cJSON_AddItemToObject(dup, "meta", detached_meta);
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        return -20;
    }
    cJSON_ReplaceItemInObject(dup, "name", new_name);

    // Print pretty representation of modified duplicate
    char *pretty = cJSON_Print(dup);
    if (pretty) {
#ifdef cJSON_free
        cJSON_free(pretty);
#else
        free(pretty);
#endif
    } else {
        // continue, but mark as non-fatal
    }

    // Add detached meta back to duplicate
    cJSON_AddItemToObject(dup, "meta", detached_meta); // dup now owns detached_meta again

    // Cleanup: delete all top-level objects we created
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);

    // If we reached this point, the sequence succeeded
    return 66;
}